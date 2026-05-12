/*==============================================================================
Сотрудники ферм с ФИО, ролью и статусом
================================================================================*/
CREATE OR REPLACE VIEW vw_farm_employee_full AS
SELECT
    fe.id AS farm_employee_id,
    f.id AS farm_id,
    f.name AS farm_name,
    p.last_name,
    p.first_name,
    p.middle_name,
    fr.name AS role_name,
    es.name AS employment_status,
    fe.hire_date,
    fe.dismissal_date,
    fe.salary,
    fe.employment_contract_number,
    fe.is_primary_workplace
FROM public.farm_employee fe
JOIN public.person p ON p.id = fe.person_id
JOIN public.farm f ON f.id = fe.farm_id
JOIN public.farm_role fr ON fr.id = fe.role_id
JOIN public.employment_status es ON es.id = fe.employment_status_id;

-- =============== ====== ====== --
-- =============== ПРИМЕР ====== --
-- =============== ====== ====== --

SELECT *
FROM vw_farm_employee_full
ORDER BY farm_name, last_name;



/*==============================================================================
Нагрузка сотрудников по участкам
================================================================================*/
CREATE OR REPLACE VIEW vw_employee_plot_load AS
SELECT
    fe.id AS farm_employee_id,
    f.name AS farm_name,
    p.last_name,
    p.first_name,
    COUNT(epa.farm_plot_id) AS active_plot_count,
    COALESCE(SUM(fp.area), 0) AS total_active_plot_area
FROM public.farm_employee fe
JOIN public.person p ON p.id = fe.person_id
JOIN public.farm f ON f.id = fe.farm_id
LEFT JOIN public.employee_plot_assignment epa
       ON epa.farm_employee_id = fe.id
      AND epa.unassigned_at IS NULL
LEFT JOIN public.farm_plot fp
       ON fp.id = epa.farm_plot_id
GROUP BY
    fe.id, f.name, p.last_name, p.first_name;

-- =============== ====== ====== --
-- =============== ПРИМЕР ====== --
-- =============== ====== ====== --

SELECT *
FROM vw_employee_plot_load
ORDER BY active_plot_count DESC;



/*==============================================================================
Заказы закупки с количеством позиций и итоговой суммой по строкам
================================================================================*/
CREATE OR REPLACE VIEW vw_purchase_order_summary AS
SELECT
    po.id,
    po.order_date,
    po.expected_delivery_date,
    po.status,
    po.association_id,
    po.supplier_id,
    COUNT(poi.id) AS item_count,
    COALESCE(SUM(poi.quantity), 0) AS total_quantity,
    COALESCE(SUM(poi.quantity * poi.unit_price * (1 + COALESCE(poi.vat_rate, 0) / 100.0)), 0) AS calculated_total,
    po.total_amount AS stored_total_amount
FROM public.purchase_order po
LEFT JOIN public.purchase_order_item poi
       ON poi.purchase_order_id = po.id
GROUP BY
    po.id, po.order_date, po.expected_delivery_date,
    po.status, po.association_id, po.supplier_id, po.total_amount;

-- =============== ====== ====== --
-- =============== ПРИМЕР ====== --
-- =============== ====== ====== --

SELECT *
FROM vw_purchase_order_summary
ORDER BY id DESC;



/*==============================================================================
Активные назначения сотрудников на участки
================================================================================*/
CREATE OR REPLACE VIEW vw_active_employee_plot_assignments AS
SELECT
    epa.id AS assignment_id,
    fe.id AS farm_employee_id,
    p.last_name,
    p.first_name,
    p.middle_name,
    f.name AS farm_name,
    fp.id AS farm_plot_id,
    fp.name AS plot_name,
    fp.area,
    epa.assignment_type,
    epa.assigned_at,
    epa.notes
FROM public.employee_plot_assignment epa
JOIN public.farm_employee fe
    ON fe.id = epa.farm_employee_id
JOIN public.person p
    ON p.id = fe.person_id
JOIN public.farm f
    ON f.id = fe.farm_id
JOIN public.farm_plot fp
    ON fp.id = epa.farm_plot_id
WHERE epa.unassigned_at IS NULL;

-- =============== ====== ====== --
-- =============== ПРИМЕР ====== --
-- =============== ====== ====== --

SELECT *
FROM vw_active_employee_plot_assignments
ORDER BY farm_name, last_name;



/*==============================================================================
Контроль контрактов
================================================================================*/
CREATE OR REPLACE VIEW vw_contract_control AS
SELECT
    c.id,
    c.contract_number,
    c.sign_date,
    c.start_date,
    c.end_date,
    c.status,
    CASE
        WHEN c.end_date IS NULL THEN 'без срока окончания'
        WHEN c.end_date < CURRENT_DATE THEN 'просрочен'
        WHEN c.end_date BETWEEN CURRENT_DATE AND CURRENT_DATE + INTERVAL '30 days' THEN 'истекает скоро'
        ELSE 'действующий'
    END AS contract_state
FROM public.contract c;

-- =============== ====== ====== --
-- =============== ПРИМЕР ====== --
-- =============== ====== ====== --

SELECT *
FROM vw_contract_control
ORDER BY end_date;