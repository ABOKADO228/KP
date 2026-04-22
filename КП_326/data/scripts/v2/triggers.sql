ROLLBACK;

/*==============================================================================
Добавление недостающих полей для работы триггеров
==============================================================================*/
ALTER TABLE public.farm_employee
ADD COLUMN IF NOT EXISTS active_plot_count integer NOT NULL DEFAULT 0;

ALTER TABLE public.farm_employee
ADD COLUMN IF NOT EXISTS is_active boolean NOT NULL DEFAULT true;

/*==============================================================================
Инициализация значений в новых полях
==============================================================================*/
UPDATE public.farm_employee fe
SET active_plot_count = (
    SELECT COUNT(*)
    FROM public.employee_plot_assignment epa
    WHERE epa.farm_employee_id = fe.id
      AND epa.unassigned_at IS NULL
);

UPDATE public.farm_employee
SET is_active = (dismissal_date IS NULL);

/*==============================================================================
Удаление старых триггеров
==============================================================================*/
DROP TRIGGER IF EXISTS trg_prevent_duplicate_active_assignment ON public.employee_plot_assignment;
DROP TRIGGER IF EXISTS trg_recalc_employee_plot_count_insupd ON public.employee_plot_assignment;
DROP TRIGGER IF EXISTS trg_recalc_employee_plot_count_del ON public.employee_plot_assignment;
DROP TRIGGER IF EXISTS trg_recalc_purchase_order_total_insupd ON public.purchase_order_item;
DROP TRIGGER IF EXISTS trg_recalc_purchase_order_total_del ON public.purchase_order_item;
DROP TRIGGER IF EXISTS trg_set_contract_status ON public.contract;
DROP TRIGGER IF EXISTS trg_set_farm_employee_inactive ON public.farm_employee;

/*==============================================================================
Удаление старых функций
==============================================================================*/
DROP FUNCTION IF EXISTS public.fn_prevent_duplicate_active_assignment() CASCADE;
DROP FUNCTION IF EXISTS public.fn_recalc_employee_plot_count() CASCADE;
DROP FUNCTION IF EXISTS public.fn_recalc_purchase_order_total() CASCADE;
DROP FUNCTION IF EXISTS public.fn_set_contract_status() CASCADE;
DROP FUNCTION IF EXISTS public.fn_set_farm_employee_inactive() CASCADE;

/*==============================================================================
Автоматический пересчет суммы заказа после добавления/изменения/удаления позиции
==============================================================================*/
CREATE OR REPLACE FUNCTION fn_recalc_purchase_order_total()
RETURNS TRIGGER
LANGUAGE plpgsql
AS $$
BEGIN
    -- INSERT
    IF TG_OP = 'INSERT' THEN
        UPDATE public.purchase_order po
        SET total_amount = COALESCE((
            SELECT SUM(quantity * unit_price * (1 + COALESCE(vat_rate, 0) / 100.0))
            FROM public.purchase_order_item poi
            WHERE poi.purchase_order_id = NEW.purchase_order_id
        ), 0)
        WHERE po.id = NEW.purchase_order_id;

        RETURN NEW;
    END IF;

    -- DELETE
    IF TG_OP = 'DELETE' THEN
        UPDATE public.purchase_order po
        SET total_amount = COALESCE((
            SELECT SUM(quantity * unit_price * (1 + COALESCE(vat_rate, 0) / 100.0))
            FROM public.purchase_order_item poi
            WHERE poi.purchase_order_id = OLD.purchase_order_id
        ), 0)
        WHERE po.id = OLD.purchase_order_id;

        RETURN OLD;
    END IF;

    -- UPDATE
    UPDATE public.purchase_order po
    SET total_amount = COALESCE((
        SELECT SUM(quantity * unit_price * (1 + COALESCE(vat_rate, 0) / 100.0))
        FROM public.purchase_order_item poi
        WHERE poi.purchase_order_id = NEW.purchase_order_id
    ), 0)
    WHERE po.id = NEW.purchase_order_id;

    IF NEW.purchase_order_id IS DISTINCT FROM OLD.purchase_order_id THEN
        UPDATE public.purchase_order po
        SET total_amount = COALESCE((
            SELECT SUM(quantity * unit_price * (1 + COALESCE(vat_rate, 0) / 100.0))
            FROM public.purchase_order_item poi
            WHERE poi.purchase_order_id = OLD.purchase_order_id
        ), 0)
        WHERE po.id = OLD.purchase_order_id;
    END IF;

    RETURN NEW;
END;
$$;

DROP TRIGGER IF EXISTS trg_recalc_purchase_order_total_insupd ON public.purchase_order_item;
CREATE TRIGGER trg_recalc_purchase_order_total_insupd
AFTER INSERT OR UPDATE ON public.purchase_order_item
FOR EACH ROW
EXECUTE FUNCTION fn_recalc_purchase_order_total();

DROP TRIGGER IF EXISTS trg_recalc_purchase_order_total_del ON public.purchase_order_item;
CREATE TRIGGER trg_recalc_purchase_order_total_del
AFTER DELETE ON public.purchase_order_item
FOR EACH ROW
EXECUTE FUNCTION fn_recalc_purchase_order_total();

-- =============== ====== ====== --
-- =============== ПРИМЕР ====== --
-- =============== ====== ====== --

BEGIN;

SELECT id, total_amount
FROM public.purchase_order
WHERE id = 2;

INSERT INTO public.purchase_order_item (
    purchase_order_id,
    product_id,
    quantity,
    unit_price,
    vat_rate,
    currency
)
VALUES (
    2,
    1,
    10.000,
    100.00,
    20.00,
    'RUB'
);

SELECT id, total_amount
FROM public.purchase_order
WHERE id = 2;

ROLLBACK;


/*==============================================================================
Автоматическая отметка просроченного контракта
==============================================================================*/
CREATE OR REPLACE FUNCTION fn_set_contract_status()
RETURNS TRIGGER
LANGUAGE plpgsql
AS $$
BEGIN
    IF NEW.end_date IS NOT NULL AND NEW.end_date < CURRENT_DATE THEN
        NEW.status := 'expired';
    END IF;

    RETURN NEW;
END;
$$;

DROP TRIGGER IF EXISTS trg_set_contract_status ON public.contract;
CREATE TRIGGER trg_set_contract_status
BEFORE INSERT OR UPDATE ON public.contract
FOR EACH ROW
EXECUTE FUNCTION fn_set_contract_status();

-- =============== ====== ====== --
-- =============== ПРИМЕР ====== --
-- =============== ====== ====== --

BEGIN;

UPDATE public.contract
SET end_date = DATE '2025-01-01'
WHERE id = 140;

SELECT id, end_date, status
FROM public.contract
WHERE id = 140;

ROLLBACK;


/*==============================================================================
Автоматическая отметка уволенного сотрудника как неактивного
==============================================================================*/
CREATE OR REPLACE FUNCTION fn_set_farm_employee_inactive()
RETURNS TRIGGER
LANGUAGE plpgsql
AS $$
BEGIN
    NEW.is_active := (NEW.dismissal_date IS NULL);
    RETURN NEW;
END;
$$;

DROP TRIGGER IF EXISTS trg_set_farm_employee_inactive ON public.farm_employee;
CREATE TRIGGER trg_set_farm_employee_inactive
BEFORE INSERT OR UPDATE ON public.farm_employee
FOR EACH ROW
EXECUTE FUNCTION fn_set_farm_employee_inactive();

-- =============== ====== ====== --
-- =============== ПРИМЕР ====== --
-- =============== ====== ====== --

BEGIN;

UPDATE public.farm_employee
SET dismissal_date = DATE '2026-04-22'
WHERE id = 1;

SELECT id, dismissal_date, is_active
FROM public.farm_employee
WHERE id = 1;

ROLLBACK;


/*==============================================================================
Автоматический пересчет количества активных участков у сотрудника
==============================================================================*/
CREATE OR REPLACE FUNCTION fn_recalc_employee_plot_count()
RETURNS TRIGGER
LANGUAGE plpgsql
AS $$
BEGIN
    -- INSERT
    IF TG_OP = 'INSERT' THEN
        UPDATE public.farm_employee fe
        SET active_plot_count = COALESCE((
            SELECT COUNT(*)
            FROM public.employee_plot_assignment epa
            WHERE epa.farm_employee_id = NEW.farm_employee_id
              AND epa.unassigned_at IS NULL
        ), 0)
        WHERE fe.id = NEW.farm_employee_id;

        RETURN NEW;
    END IF;

    -- DELETE
    IF TG_OP = 'DELETE' THEN
        UPDATE public.farm_employee fe
        SET active_plot_count = COALESCE((
            SELECT COUNT(*)
            FROM public.employee_plot_assignment epa
            WHERE epa.farm_employee_id = OLD.farm_employee_id
              AND epa.unassigned_at IS NULL
        ), 0)
        WHERE fe.id = OLD.farm_employee_id;

        RETURN OLD;
    END IF;

    -- UPDATE
    UPDATE public.farm_employee fe
    SET active_plot_count = COALESCE((
        SELECT COUNT(*)
        FROM public.employee_plot_assignment epa
        WHERE epa.farm_employee_id = NEW.farm_employee_id
          AND epa.unassigned_at IS NULL
    ), 0)
    WHERE fe.id = NEW.farm_employee_id;

    IF NEW.farm_employee_id IS DISTINCT FROM OLD.farm_employee_id THEN
        UPDATE public.farm_employee fe
        SET active_plot_count = COALESCE((
            SELECT COUNT(*)
            FROM public.employee_plot_assignment epa
            WHERE epa.farm_employee_id = OLD.farm_employee_id
              AND epa.unassigned_at IS NULL
        ), 0)
        WHERE fe.id = OLD.farm_employee_id;
    END IF;

    RETURN NEW;
END;
$$;

DROP TRIGGER IF EXISTS trg_recalc_employee_plot_count_insupd ON public.employee_plot_assignment;
CREATE TRIGGER trg_recalc_employee_plot_count_insupd
AFTER INSERT OR UPDATE ON public.employee_plot_assignment
FOR EACH ROW
EXECUTE FUNCTION fn_recalc_employee_plot_count();

DROP TRIGGER IF EXISTS trg_recalc_employee_plot_count_del ON public.employee_plot_assignment;
CREATE TRIGGER trg_recalc_employee_plot_count_del
AFTER DELETE ON public.employee_plot_assignment
FOR EACH ROW
EXECUTE FUNCTION fn_recalc_employee_plot_count();

-- =============== ====== ====== --
-- =============== ПРИМЕР ====== --
-- =============== ====== ====== --

BEGIN;

SELECT id, active_plot_count
FROM public.farm_employee
WHERE id = 1;

INSERT INTO public.employee_plot_assignment (
    farm_employee_id,
    farm_plot_id,
    assignment_type,
    assigned_at,
    unassigned_at,
    notes
)
VALUES (
    1,
    1,
    'temporary',
    DATE '2026-04-22',
    NULL,
    'Демо для проверки триггера'
);

SELECT id, active_plot_count
FROM public.farm_employee
WHERE id = 1;

ROLLBACK;


/*==============================================================================
Запрет повторного активного назначения сотрудника на тот же участок
==============================================================================*/
CREATE OR REPLACE FUNCTION fn_prevent_duplicate_active_assignment()
RETURNS TRIGGER
LANGUAGE plpgsql
AS $$
BEGIN
    IF NEW.unassigned_at IS NULL THEN
        IF EXISTS (
            SELECT 1
            FROM public.employee_plot_assignment epa
            WHERE epa.farm_employee_id = NEW.farm_employee_id
              AND epa.farm_plot_id = NEW.farm_plot_id
              AND epa.unassigned_at IS NULL
              AND (TG_OP = 'INSERT' OR epa.id <> NEW.id)
        ) THEN
            RAISE EXCEPTION 'У сотрудника уже есть активное назначение на этот участок';
        END IF;
    END IF;

    RETURN NEW;
END;
$$;

DROP TRIGGER IF EXISTS trg_prevent_duplicate_active_assignment ON public.employee_plot_assignment;
CREATE TRIGGER trg_prevent_duplicate_active_assignment
BEFORE INSERT OR UPDATE ON public.employee_plot_assignment
FOR EACH ROW
EXECUTE FUNCTION fn_prevent_duplicate_active_assignment();

-- =============== ====== ====== --
-- =============== ПРИМЕР ====== --
-- =============== ====== ====== --

BEGIN;

INSERT INTO public.employee_plot_assignment (
    farm_employee_id,
    farm_plot_id,
    assignment_type,
    assigned_at,
    unassigned_at,
    notes
)
VALUES (
    1,
    128,
    'primary',
    DATE '2026-04-22',
    NULL,
    'Демо проверки запрета дубля'
);

ROLLBACK;