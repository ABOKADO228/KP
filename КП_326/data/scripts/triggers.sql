/*==============================================================================
Автоматический пересчет суммы заказа после добавления/изменения/удаления позиции
================================================================================*/
CREATE OR REPLACE FUNCTION fn_recalc_purchase_order_total()
RETURNS TRIGGER
LANGUAGE plpgsql
AS $$
DECLARE
    v_order_id integer;
BEGIN
    v_order_id := COALESCE(NEW.purchase_order_id, OLD.purchase_order_id);

    UPDATE public.purchase_order po
    SET total_amount = COALESCE((
        SELECT SUM(quantity * unit_price * (1 + COALESCE(vat_rate, 0) / 100.0))
        FROM public.purchase_order_item poi
        WHERE poi.purchase_order_id = v_order_id
    ), 0)
    WHERE po.id = v_order_id;

    RETURN COALESCE(NEW, OLD);
END;
$$;

CREATE TRIGGER trg_recalc_purchase_order_total_insupd
AFTER INSERT OR UPDATE ON public.purchase_order_item
FOR EACH ROW
EXECUTE FUNCTION fn_recalc_purchase_order_total();

CREATE TRIGGER trg_recalc_purchase_order_total_del
AFTER DELETE ON public.purchase_order_item
FOR EACH ROW
EXECUTE FUNCTION fn_recalc_purchase_order_total();

/*==============================================================================
Автоматическая отметка просроченного контракта
================================================================================*/
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

CREATE TRIGGER trg_set_contract_status
BEFORE INSERT OR UPDATE ON public.contract
FOR EACH ROW
EXECUTE FUNCTION fn_set_contract_status();

/*==============================================================================
Автоматическая отметка уволенного сотрудника как неактивного
================================================================================*/
CREATE OR REPLACE FUNCTION fn_set_farm_employee_inactive()
RETURNS TRIGGER
LANGUAGE plpgsql
AS $$
BEGIN
    IF NEW.dismissal_date IS NOT NULL THEN
        NEW.is_active := FALSE;
    END IF;

    RETURN NEW;
END;
$$;

CREATE TRIGGER trg_set_farm_employee_inactive
BEFORE INSERT OR UPDATE ON public.farm_employee
FOR EACH ROW
EXECUTE FUNCTION fn_set_farm_employee_inactive();

/*==============================================================================
Автоматический пересчет количества активных участков у сотрудника
================================================================================*/
CREATE OR REPLACE FUNCTION fn_recalc_employee_plot_count()
RETURNS TRIGGER
LANGUAGE plpgsql
AS $$
DECLARE
    v_employee_id integer;
BEGIN
    v_employee_id := COALESCE(NEW.farm_employee_id, OLD.farm_employee_id);

    UPDATE public.farm_employee fe
    SET active_plot_count = COALESCE((
        SELECT COUNT(*)
        FROM public.employee_plot_assignment epa
        WHERE epa.farm_employee_id = v_employee_id
          AND epa.unassigned_at IS NULL
    ), 0)
    WHERE fe.id = v_employee_id;

    RETURN COALESCE(NEW, OLD);
END;
$$;

CREATE TRIGGER trg_recalc_employee_plot_count_insupd
AFTER INSERT OR UPDATE ON public.employee_plot_assignment
FOR EACH ROW
EXECUTE FUNCTION fn_recalc_employee_plot_count();

CREATE TRIGGER trg_recalc_employee_plot_count_del
AFTER DELETE ON public.employee_plot_assignment
FOR EACH ROW
EXECUTE FUNCTION fn_recalc_employee_plot_count();

/*==============================================================================
Запрет повторного активного назначения сотрудника на тот же участок
================================================================================*/
CREATE OR REPLACE FUNCTION fn_prevent_duplicate_active_assignment()
RETURNS TRIGGER
LANGUAGE plpgsql
AS $$
BEGIN
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

    RETURN NEW;
END;
$$;

CREATE TRIGGER trg_prevent_duplicate_active_assignment
BEFORE INSERT OR UPDATE ON public.employee_plot_assignment
FOR EACH ROW
EXECUTE FUNCTION fn_prevent_duplicate_active_assignment();